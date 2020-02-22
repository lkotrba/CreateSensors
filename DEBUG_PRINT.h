//#ifndef DEBUGUTILS_H
//#define DEBUGUTILS_H

#ifdef DEBUG
  #define DEBUG_PRINT(x)        Serial.print (x)
  #define DEBUG_PRINTDEC(x)     Serial.print (x, DEC)
  #define DEBUG_PRINTLN(x)      Serial.println (x)

  #define DEBUG_PRINTVAL(x)              \
         Serial.print (#x);              \
         Serial.print (" =: ");          \
         Serial.println (x)                 

  #define DEBUG_PRINTVAL_DEL(x)          \
         Serial.print (#x);              \
         Serial.print (" =: ");          \
         Serial.println (x);             \
         delay (5)    

  #define DEBUG_PRINT_IDENT(x)           \
         Serial.print(millis());         \
         Serial.print(": ");             \
         Serial.print(__FUNCTION__);     \
         Serial.print("() in ");         \
         Serial.print(__FILE__);         \
         Serial.print(":");              \
         Serial.print(__LINE__);         \
         Serial.print(" ");              \
         Serial.print(#x);               \
         Serial.print(" =: ");           \
         Serial.println(x)

       
#else
  #define DEBUG_PRINT(x) 
  #define DEBUG_PRINTDEC(x)
  #define DEBUG_PRINTLN(x) 
  #define DEBUG_PRINTVAL(x)
  #define DEBUG_PRINTVAL_DEL(x)  
  #define DEBUG_PRINT_IDENT(x)           

#endif

//#endif
