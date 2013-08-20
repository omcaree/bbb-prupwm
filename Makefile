CXXFLAGS=-Wall -I./include

all: prupwm

prupwm: main.cpp src/pruPWM.o src/pru.o include/pruPWM.h include/pru.h pwm.bin
	g++ $(CXXFLAGS) main.cpp src/pruPWM.o src/pru.o -lprussdrv -lpthread -o prupwm

pwm.bin: src/pwm.p include/pru.hp
	cd src; pasm -b pwm.p; mv pwm.bin ../; cd ..
	
clean:
	rm pwm.bin prupwm src/*.o