
#include "BinaryFileTask.h"
#include "base/EmComm.h"
#include "pathhelper/PathHelper.h"

BinaryFileTask::BinaryFileTask(const char *url, const char *save, int cap)
	:BinaryStreamTask(url, cap)
	,_save(save){

}

void BinaryFileTask::onFinalized(CURLcode res, long statusCode) {
	BinaryStreamTask::onFinalized(res, statusCode);
	if(0==res && 2 == (statusCode/100)) {
        //----- NEED THE ABSOLUTE PATH NOW.
        std::string prepath = PathHelper::getInstance().getWritablePath();
        //prepath.append("/");
        prepath.append(_save);
		saveToPath(prepath.c_str());
	} else {
		fprintf(stderr,"Failed to download %s\n", _save.c_str());
	}
}

std::string BinaryFileTask::toStr() {
	char buf[BUFSIZ];
	snprintf(buf, sizeof(buf), "Downloading<%s>", _save.c_str());
	return buf;
}
