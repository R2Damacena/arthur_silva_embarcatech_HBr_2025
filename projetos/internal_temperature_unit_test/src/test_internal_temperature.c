#include "unity.h"
#include <math.h>

// Prototipar a função a ser testada
float adc_to_celsius(uint16_t adc_val);

void setUp(void)
{
    // set stuff up here
}

void tearDown(void)
{
    // clean stuff up here
}

void test_adc_to_celsius_known_value(void) {
    // Para 0.706V, temp = 27°C
    // adc_val = (0.706 / 3.3) * 4096 ≈ 876
    uint16_t adc_val = 876;
    float temp = adc_to_celsius(adc_val);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 27.0f, temp); // margem de erro de 0.1°C
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_adc_to_celsius_known_value);
    return UNITY_END();
}