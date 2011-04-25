make = nmake /nologo

all:
	cd Client
	$(make)
	cd ..\Server
	$(make)
	cd ..

clean:
	cd Client
	$(make) clean
	cd ..\Server
	$(make) clean
	cd ..
