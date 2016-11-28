

// August. 2o15
// Authored by Eminem


#include "ThreadPool.h"
#include "Workload.h"
#include "AsyncNotifier.h"
#include "base/LogComm.h"
#include <cassert>
#include <algorithm>
#include <iostream>
#include <sstream>

static ThreadPool* _instance;

#define AssignProtected(o, mem, val)	\
	do{\
		std::lock_guard<decltype(o->mutex)> lock(o->mutex);\
		o->mem = val;\
	}while(0);\

#if defined(_DEBUG) || defined(DEBUG)

	//printf("Thread<%lu> is processing\n", std::this_thread::get_id());
	//printf("Thread<%lu> finished\n", std::this_thread::get_id());

#define OutputBeforeProcess()\
	do{\
		std::ostringstream os;\
		os<<"Thread<"<<std::this_thread::get_id()<<"> is processing\n";\
		std::cout<<os.str();\
	}while(0);

#define OutputAfterProcess()\
	do{\
		std::ostringstream os;\
		os<<"Thread<"<<std::this_thread::get_id()<<"> has finished"<<std::endl;\
		std::cout<<os.str();\
	}while(0);

#else
#define OutputBeforeProcess()	(void*)0;
#define OutputAfterProcess()	(void*)0;
#endif

ThreadPool::ThreadPool():_stopped(0)
{
}

ThreadPool::~ThreadPool(){ }

ThreadPool* ThreadPool::getInstance() {
	if(!_instance) {
		_instance = new ThreadPool;
	}
	return _instance;
}

void ThreadPool::start(int tcount) {
	_stopped = 0;
    for(int i = 0; i < tcount; ++i) {
        auto u = new ThreadUnit;
        u->thread = std::thread([=]() {
            this->listenNdAccept(u);
        });
        _threads.push_back(u);
    }
}

void ThreadPool::stopPool() {
	LOGE("Notified of <HALT> of this thread-pool");
	_stopped = 1;	//~ MARK OFFICIALLY
    _condiv.notify_all();
    for(auto tu:_threads) {
		do{
            std::lock_guard<decltype(tu->mutex)> guard(tu->mutex);
            if(tu->assignment) {
                tu->assignment->cancel();
            }
        }while(0);
        if(tu->thread.joinable()) {
            tu->thread.join();
        }
        delete tu;
    }
    _threads.clear();
}

void ThreadPool::assign(Workload* work) {
    std::lock_guard<decltype(_tmutex)> lock(_tmutex);
    _assignments.push_back(work);
    _condiv.notify_one();
}

void ThreadPool::cancel(Workload *work) {
	std::lock_guard<decltype(_tmutex)> lock(_tmutex);
	if (work->_isProcessing) {
		work->_isCanceling = 1;
	} else {
		auto pos = std::find(_assignments.begin(), _assignments.end(), work);
		if (pos!=_assignments.end()){
			_assignments.erase(pos);
			work->cancel();	//~ Which kill itself
		}
	}
}

void ThreadPool::listenNdAccept(ThreadUnit* tUnit) {
	do {
        Workload* now = nullptr;
        do{
            std::unique_lock<decltype(_tmutex)> lock(_tmutex);
			// Wake up if thread-pool is stopped or some new assignment is in queue.
            _condiv.wait(lock, [this](){ return !_assignments.empty() || _stopped; });
			//~ There is a big difference writing bewteen if stopped and if stopped != 0
			//~ a type of atom_char(operator overloaded)
            if (_stopped != 0)  {
				int stopVal = _stopped;
				LOGE("Thread-pool is forced to halt %d", stopVal);
                break;
            }
            assert(!_assignments.empty());
            now = _assignments.front();
            _assignments.pop_front();
			now->_isProcessing = 1;
        }while(0);
		//OutputBeforeProcess();
		AssignProtected(tUnit, assignment, now)
		//LOGE("Before first execute:%p", now);
        now->execute();
		//LOGE("After the execution");
		AsyncNotifier::getInstance()->schedule([=]{now->finish();});
		AssignProtected(tUnit, assignment, nullptr)
		//OutputAfterProcess()
    }while(true);
}
