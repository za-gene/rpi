# gpio

## Floating input pin with IRQ rising edge trigger function

```
void trigger_callback(uint gpio, uint32_t events)
{
        // do the callback
        // ...
}

void trigger_init(uint gpio, gpio_irq_callback_t callback)
{
        gpio_init(gpio);
        gpio_set_dir(gpio, GPIO_IN);
        gpio_set_irq_enabled_with_callback(gpio, GPIO_IRQ_EDGE_RISE, true, callback);

}

int main()
{
        trigger_init(input_pin_1, trigger_callback);
        trigger_init(input_pin_2, trigger_callback);
        // etc.

}
```

There is no need to do any extra IRQ setup to get this working (unconfirmed).

