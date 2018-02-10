#pragma once
class SettingItem
{
private:
	
	unsigned short int value;
	unsigned short int max_value;
	unsigned short int min_value;
public:
	char advanced[3][6];
	char settingname[30];
	void setrange(unsigned short int max, unsigned short int min);
	void changevalue(short int val);
	unsigned short int getvalue();
	SettingItem *nextSettingItem;
	SettingItem();
	~SettingItem();
};
