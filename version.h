#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "17";
	static const char MONTH[] = "07";
	static const char YEAR[] = "2009";
	static const double UBUNTU_VERSION_STYLE = 9.07;
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 5;
	static const long BUILD = 603;
	static const long REVISION = 3233;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 1338;
	#define RC_FILEVERSION 1,5,603,3233
	#define RC_FILEVERSION_STRING "1, 5, 603, 3233\0"
	static const char FULLVERSION_STRING[] = "1.5.603.3233";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 31;
	

}
#endif //VERSION_H
