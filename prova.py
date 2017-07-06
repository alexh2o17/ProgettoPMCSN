
import os
import time
for i in range (100):
	for j in range(400):
		time.sleep(1)
		os.system("./prova_out")
	os.system("./confidenza_out")
	os.remove("./T_Risposta")
	os.remove("./Throughput")
	os.remove("./Utilizzazione")

