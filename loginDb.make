all: ../public_html/cgi-bin/loginDb.cgi\
	 ../public_html/cgi-bin/registerDb.cgi\
	 ../public_html/cgi-bin/setPermissionsDb.cgi
# ###########################################
# mysql stuff
# ###########################################

mysqlib = -L/usr/lib/mysql
mysqlinc = -I/usr/include/mysql
mysqliblk = -lmysqlclient

####################################################################
# VARIABLES FOR GENERAL STUFF
####################################################################

lgeno = genRoutinesDb.o cgin.o gRout2Db.o
lgenh = genRoutinesDb.hpp cgin.hpp gRout2Db.hpp

####################################################################

# ###########################################
# Linking
# ###########################################
	
../public_html/cgi-bin/loginDb.cgi: loginDb.o database.o queries.o $(lgeno)
	g++ $(mysqlib) -o ../public_html/cgi-bin/loginDb.cgi loginDb.o database.o queries.o $(mysqliblk) $(lgeno)
	
../public_html/cgi-bin/registerDb.cgi: registerDb.o database.o queries.o $(lgeno)
	g++ $(mysqlib) -o ../public_html/cgi-bin/registerDb.cgi registerDb.o database.o queries.o $(mysqliblk) $(lgeno)
	
../public_html/cgi-bin/setPermissionsDb.cgi: setPermissionsDb.o database.o queries.o $(lgeno)
	g++ $(mysqlib) -o ../public_html/cgi-bin/setPermissionsDb.cgi setPermissionsDb.o database.o queries.o $(mysqliblk) $(lgeno)
	
	

# ###########################################
# Compilation of files for each script
# ###########################################

loginDb.o: loginDb.cpp
	g++ -c loginDb.cpp

registerDb.o: registerDb.cpp
	g++ -c registerDb.cpp

database.o: database.cpp database.hpp
	g++ $(mysqlinc) -c database.cpp

cgin.o: cgin.cpp cgin.hpp
	g++ -c cgin.cpp

genRoutinesDb.o: genRoutinesDb.cpp genRoutinesDb.hpp 
	g++ -c genRoutinesDb.cpp 

gRoutDb.o: gRoutDb.cpp gRoutDb.hpp 
	g++ -c gRoutDb.cpp

queries.o: queries.cpp queries.hpp 
	g++ -c queries.cpp
	
setPermissionsDb.o: setPermissionsDb.cpp
	g++ $ -c setPermissionsDb.cpp

