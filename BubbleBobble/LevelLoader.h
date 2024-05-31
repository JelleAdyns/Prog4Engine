#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <string>
#include"LevelState.h"

namespace levelLoader
{
	void LoadLevel(const std::string& filename, int levelNumber);
}


#endif // !LEVELLOADER_H