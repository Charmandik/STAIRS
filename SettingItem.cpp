#include "SettingItem.h"

 void SettingItem::setrange(unsigned char min, unsigned char max)
 {
	 min_value = min;
	 max_value = max;	
 }

 void SettingItem::setvalue(unsigned char val)
 {
	 if (val < min_value)  val = min_value;
	 if (val > max_value)  val = max_value;
	 value = val;
 }

 void SettingItem::changevalue(short val) //increment or dicrement only 1==val
 {
	 value += val;
	 if (value < min_value)  value = min_value; 
	 if (value > max_value)  value = max_value; 	  
 }
  
 SettingItem::SettingItem():value(0)
{
	min_value = 0;
	max_value = 255;
}


SettingItem::~SettingItem()
{
}
