#if YF_DEBUG
UINT32 XCJ_Printf(const char *fmt, ...)
{		
	 va_list args;					
	 int result;					
	 va_start(args, fmt);			 	
	 result = vprintf(fmt, args);	  
	 va_end(args);					  
	 return result;  
}	
#else
UINT32 XCJ_Printf(const char *fmt, ...)
{
	 return 0;
}
#endif