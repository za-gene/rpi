;(foreign-declare "#include \"/home/pi/pico/pico-sdk/src/rp2_common/hardware_gpio/include/hardware/gpio.h\"")
;(foreign-declare "#include \"/home/pi/pico/pico-sdk/src/rp2_common/hardware_timer/include/hardware/timer.h\"")

(define wait-ms 
  (foreign-lambda
    void "busy_wait_ms" int int))


(define (gpio-out gpio) 1)
(define (gpio-put gpio value) 1)

;(define (wait-ms ms)
;  (foreign-lambda void busy_wait_ms ms))

(define gpio-init (foreign-lambda void "gpio_init" int))

(define gpio-set-dir (foreign-lambda void "gpio_init" int int))

(define (gpio-out gpio)
  (gpio-init gpio)
  (gpio-set-dir gpio 1))
;  (foreign-lambda void gpio_set_dir gpio 1))

(define gpio-put
  (foreign-lambda void "gpio_put" int int))


(define led 13)

(gpio-out led)

(let loop ()
  (gpio-put led 1)
  (wait-ms 100)
  (gpio-put led 0)
  (wait-ms 900))

