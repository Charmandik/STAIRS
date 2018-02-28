#pragma once
class SettingItem
{
private:
public:
	short value;
	unsigned char max_value;
	unsigned char  min_value;
	void setrange(unsigned char max, unsigned char min);
	void setvalue(unsigned char val);
	void changevalue(short val);
	SettingItem *nextSettingItem;	
	SettingItem();
	~SettingItem();
};
