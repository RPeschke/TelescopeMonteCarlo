#ifndef TelescopeSetup_mc_h__
#define TelescopeSetup_mc_h__


class TelescopeSetup{
public:
	TelescopeSetup();
	void setSaveFile(const char* fileName);
	void readXMLConfig(const char* fileName);
	void Run(int NumberOfEvents);
	~TelescopeSetup();

private: 
	class PImpl;
	PImpl *privatImpl;

};

#endif // TelescopeSetup_mc_h__
