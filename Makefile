########################################
##
## Makefile
## LINUX compilation
##
##############################################

#FLAGS
C++FLAG = -g -std=c++11 -Wall

#Math Library
MATH_LIBS = -lm
EXEC_DIR=.


#Rule for .cpp files
# .SUFFIXES : .cc.o
.cc.o:
	g++ $(C++FLAG) $(INCLUDES)  -c $< -o $@


#Including
INCLUDES=  -I.

LIBS_ALL =  -L/usr/lib -L/usr/local/lib $(MATH_LIBS)

#INSERT AND SEARCH PROGRAM
ALL_OBJ0=test_insert_and_search.o
PROGRAM_0=test_insert_and_search
$(PROGRAM_0): $(ALL_OBJ0)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(ALL_OBJ0) $(INCLUDES) $(LIBS_ALL)

#INSERT AND DELETE PROGRAM
ALL_OBJ1=test_insert_and_delete.o
PROGRAM_1=test_insert_and_delete
$(PROGRAM_1): $(ALL_OBJ1)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(ALL_OBJ1) $(INCLUDES) $(LIBS_ALL)

#NEW INSERT PROGRAM
ALL_OBJ2=test_new_insert.o
PROGRAM_2=test_new_insert
$(PROGRAM_2): $(ALL_OBJ2)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(ALL_OBJ2) $(INCLUDES) $(LIBS_ALL)


#Compile all
all:
	make $(PROGRAM_0)
	make $(PROGRAM_1)
	make $(PROGRAM_2)


#Clean obj and exec files
clean:
	(rm -f *.o; rm -f $(PROGRAM_0); rm -f $(PROGRAM_1); rm -f $(PROGRAM_2))

(:
