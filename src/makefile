.PHONY: clean 
PRODUCTS = ds
default: $(PRODUCTS)

OBJDIR = ../obj

#to make objects
include bst/makefile
include cdll/makefile
include node/makefile
#unittest
include unittest/makefile
include unittest/bst/makefile
include unittest/cdll/makefile


#All objects
OBJS = $(OBJDIR)/ds_bst.o 
OBJS += $(OBJDIR)/ds_cdll.o

JUNK = *~ $(OBJDIR)/*.o $(OBJDIR)/*.a  

# Define the tools we are going to use
CC= gcc
LD = gcc
CFLAGS = -g -Wall -ansi -I.   

$(PRODUCTS): $(OBJS)
	@echo "Congratulations, all objects have been made"
	
#unittest,using googletest:https://github.com/google/googletest/blob/master/googletest/docs/Primer.md
#this make no sense, and just test googletest

UT = $(OBJDIR)/ds_bst_ut
UT += $(OBJDIR)/ds_cdll_ut

ut-all: $(UT)
	./$(OBJDIR)/ds_bst_ut
	./$(OBJDIR)/ds_cdll_ut

ut-bst: $(OBJDIR)/ds_bst_ut
	./$(OBJDIR)/ds_bst_ut

ut-cdll: $(OBJDIR)/ds_cdll_ut
	./$(OBJDIR)/ds_cdll_ut


clean:
	@rm -f $(JUNK) $(PRODUCTS) 


