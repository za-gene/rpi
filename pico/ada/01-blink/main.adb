--
--  Copyright 2021 (C) Jeremy Grosser
--
--  SPDX-License-Identifier: BSD-3-Clause
--
with RP.Device;
with RP.Clock;
with RP.GPIO;
with Pico;

procedure Main is
begin
	RP.Clock.Initialize (Pico.XOSC_Frequency);
	Pico.LED.Configure (RP.GPIO.Output);
	RP.Device.Timer.Enable;
	loop
		Pico.LED.Set;
		RP.Device.Timer.Delay_Milliseconds (100);
		Pico.LED.Clear;
		RP.Device.Timer.Delay_Milliseconds (900);
	end loop;
end Main;
