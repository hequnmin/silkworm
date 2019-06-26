

#include "FileUtil.h"

FileUtil::FileUtil() {
  SPIFFS.begin();
}

FileUtil::~FileUtil() {
  
}

bool FileUtil::isExists(String fileName) {
  bool exist = false;
  
  exist = SPIFFS.exists(fileName);
  
  return exist;
}

