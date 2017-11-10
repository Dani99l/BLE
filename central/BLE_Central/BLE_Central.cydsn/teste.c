/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "cytypes.h"
#include "teste.h"
/* [] END OF FILE */
static char _buf[30];
 
char* ultoa( uint32 val) {
  char* p;
 
  p = _buf + sizeof(_buf);
  *--p = '\0';
  do {
      *--p = '0' + val%10;
      val /= 10;
  } while (val);
  return p;
}
 
char* ltoa(int32 val) {
  char* p;
  uint16 flg = 0;
 
  if (val < 0){
      flg++;
      val = -val;
  }
  p = ultoa(val);
  if (flg) *--p = '-';
  return p;
}