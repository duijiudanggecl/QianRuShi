assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
	#define IS_GPIO_ALL_INSTANCE(PERIPH) (((PERIPH) == GPIOA) || ((PERIPH) == GPIOB))

	assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))
		void assert_failed(uint8_t* file, uint32_t line)
		{
    			printf("FILE:%s, LINE: %u\n", file, line);
    			while(1);
		}