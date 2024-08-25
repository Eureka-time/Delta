﻿#ifndef INI_FILE_CPP_H_
#define INI_FILE_CPP_H_

using namespace std;

class CIni
{
public:
	CIni(const string & fileName);
public:
	virtual ~CIni(void);
	
	const string & getFileName() const;

	const string &getSection() const;
	void setSection(const string &section);
	
	bool write(const string &key, const string & value) const ;
	bool write(const string &key, int value) const ;
	bool write(const string &key, double value) const;

	string readStr(const string &key,const string &default_value) const;
	int readInt(const string &key, int default_value) const;
	double readDouble(const string &key, double default_value) const;
	float GetFloat(const string &section, const string &key);
	bool GetBool(const string &section, const string &key);
	string GetStr(const string &section, const string &key);
	int GetInt(const string &section, const string &key);
	double GetDouble(const string &section, const string &key);
    bool CheckKeyExist(const string &section, const string &key);

public:
	static int read_profile_string( const char *section, const char *key,char *value, 
		int size, const char *default_value, const char *file);
	static int read_profile_int( const char *section, const char *key,int default_value, 
		const char *file);
	static double read_profile_double(const char *section, const char *key, double default_value,
		const char *file);
	static int write_profile_string(const char *section, const char *key,
		const char *value, const char *file);

private:
	static int load_ini_file(const char *file, char *buf,int *file_size);
	static int newline(char c);
	static int end_of_string(char c);
	static int left_barce(char c);
	static int right_brace(char c );
	static int parse_file(const char *section, const char *key, const char *buf,int *sec_s,int *sec_e,
		int *key_s,int *key_e, int *value_s, int *value_e);

private:
	string m_fileName;
	string m_section;
};

#endif

