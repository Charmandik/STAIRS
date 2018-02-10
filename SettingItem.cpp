#include "SettingItem.h"

 void SettingItem::setrange(unsigned short int min, unsigned short int max)
 {
	 min_value = min;
	 max_value = max;	
 }

 void SettingItem::changevalue( short int val)
 {
	 if (value < min_value)  value = min_value; 
	 if (value > max_value)  value = max_value; 	
	 value += val;	 
 }

 unsigned short int SettingItem::getvalue()
 {
	 return this->value;
 }

SettingItem::SettingItem():value(0)
{
	min_value = 0;
	max_value = 255;
}


SettingItem::~SettingItem()
{
}
