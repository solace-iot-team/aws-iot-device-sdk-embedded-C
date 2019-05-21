o build the scc demo application through linux command line:
-------------------------------------------------------------

1. Install libssl-dev through apt-get
	`sudo apt-get install libssl-dev`
2. If there is an error saying "opensslconf.h" missing, then create the symbolic link for it as below:
	`sudo ln -s /usr/include/arm-linux-guneabihf**/openssl/opensslconf.h /usr/include/openssl`
3. Download the raspberry-pi3+ arm cross compiler version 8.3 from below website:
	https://protect-us.mimecast.com/s/yj15CXDwEpFqyG4H6Jxp2?domain=sourceforge.net
4. Uncompress to local directory (e.g. /home/pi/cross-xxx)
5. Copy the library files and openssl directory to your local cross-compiler's library and include directory respectively.
	`cp libssl* libcrypto* libscc-toolkit* libpcsclite* <local cross-compiler dir>/arm-linux-gnueabihf/lib/`
	`cp -r openssl <local cross-compiler dir>/arm-linux-gnueabihf/include/`
6. Create the soft-l	ink to your local directory to /opt/
	`sudo ln -s /home/pi/cross-xxx /opt/`
7. Download scc demo application directory
8. Execute the scc demo app as below:. 
	`arm-linux-gnueabihf-g++ demo-src/demo3.cpp -lssl -lcrypto -lscc-toolkit -I./inc/ -std=c++17 -o demo3`
	

To build the scc demo application through EclipseIDE 4.11.0(Linux):
-------------------------------------------------------------------

1. Follow the command line steps (1) to (7)
2. Launch Eclipse IDE
3. File -> New -> C++ Project -> C++ Managed Build
4. Provide the project name, and select below:
	- Project type -> Empty Project
	- Toolchains -> Cross GCC
	- Click 'Next' 2 times
5. In Cross GCC command dialog box, enter below:
	- Cross compiler prefix -> arm-linux-gnueabihf-
	- Cross compiler path -> /opt/cross-pi-gcc-8.3.0-2/bin
	- Click "Finish"
6. Import the scc demo directory where source and include files are present
	- File -> Import -> File systems -> Browse and select the appropriate directory
	- Click "Finish"
7. Click on the Project name -> Right click -> Select "Properties"
8. C/C++ Build -> Settings -> Cross G++ Compiler -> Dialect -> Other dialect flags -> "-std=c++17"
												 -> Includes -> Include Paths (-I) -> click (+) symbol to add new include path -> Select Workspace -> "Select your project"/inc
						   -> Cross G++ Linker -> Libraries -> Libraries (-l) -> click (+) symbol to add new libraries -> create three libraries (ssl, cryto, scc-toolkit)
9. Build the project - Right click on the Project name -> Build the project
10. Launch the executable (Project name -> Debug -> Project name -> xxx.o)


