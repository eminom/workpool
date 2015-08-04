
#include "BinaryFileTask.h"
#include "EmComm.h"

BinaryFileTask::BinaryFileTask(const char *url, const char *save, int cap)
	:BinaryStreamTask(url, cap)
	,_save(save){

}

void BinaryFileTask::onFinalized(CURLcode res) {
	BinaryStreamTask::onFinalized(res);
	if(0==res) {
		int length = write_ptr_ - begin_ptr_;
		if( FILE *fout = fopen(_save.c_str(), "wb") ) {
			fwrite(begin_ptr_, 1, length, fout);
			fclose(fout);
			printf("binary stream saved to %s\n", _save.c_str());
		}
	} else {
		fprintf(stderr,"Failed to download %s\n", _save.c_str());
	}
}

std::string BinaryFileTask::toStr() {
	char buf[BUFSIZ];
	snprintf(buf, sizeof(buf), "Downloading to %s", _save.c_str());
	return buf;
}
