jobscheduler: jobscheduler.o
	g++ -o jobscheduler jobscheduler.o
jobscheduler.o: jobscheduler.cpp
	g++ -c jobscheduler.cpp