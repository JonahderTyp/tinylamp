VCC = 4.2
FC = 0.06
FV = 3.3
LED_IN_PARALLEL = 2


print("Resistor:", "{:.2f}".format((VCC-FV)/FC), "ohm")
print("POWER", "{:.2f}".format(FC*LED_IN_PARALLEL*(VCC-FV)*1000), "mW")


