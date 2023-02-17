CXX = g++
TARGET_DIR =../bin
UNGINE_SDK_PATH = ../
CXXFLAGS = -std=c++14 -m64 -march=athlon64 -msse -msse2 -msse4.2 -ffast-math -fno-strict-aliasing -Wall -I"../include"  -D_LINUX
LDFLAGS = -Wl,-rpath,'$$ORIGIN' -Wl,--no-as-needed -L$(UNGINE_SDK_PATH)bin -lgsl -lm -lgslcblas
DEPEND=./.depend
ifeq ($(shell arch),x86_64)
	ARCH:=x64
else
	ARCH:=x86
endif

SRCS =\
	main.cpp\
	AppWorldLogic.cpp\
	AppEditorLogic.cpp\
	AppSystemLogic.cpp\
	PlayerController.cpp\
	LevelManager.cpp\
	MovementControls.cpp\
	ProcJson.cpp

OBJS = $(SRCS:.cpp=.o)

TARGET=$(TARGET_DIR)/Test_Task_Stratus_$(ARCH)
UNIGINE_LIB=Unigine_$(ARCH)

MAKEDEPEND=set -e; \
	$(CXX) $(CXXFLAGS) -MM -o $*.Td $<; \
	mkdir -p $(dir $(DEPEND)/$*.d); \
	cp $*.Td $(DEPEND)/$*.d; \
	sed -e 's/\#.*//' \
		-e 's/^[^:]*: *//' \
		-e 's/ *\\$$//' \
		-e '/^$$/ d' \
		-e 's/$$/ :/' \
		< $*.Td >> $(DEPEND)/$*.d; \
	rm -f $*.Td

#
# Debug build settings
#
DBGDIR = debug
DBGAPP = $(TARGET)d
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJS))
DBGCXXFLAGS =-g -DDEBUG
DBGLIB=-l$(UNIGINE_LIB)d

#
# Release build settings
#
RELDIR = release
RELAPP = $(TARGET)
RELOBJS = $(addprefix $(RELDIR)/, $(OBJS))
RELCXXFLAGS=-O2 -DNDEBUG
RELLIB=-l$(UNIGINE_LIB)

.PHONY: all clean debug prep release rebuild

# Default build
all: release

#
# Debug rules
#
debug: prep $(DBGAPP)

$(DBGAPP): $(DBGOBJS)
	$(CXX) $^ $(LDFLAGS) $(DBGLIB) -o $@

$(DBGOBJS): $(DBGDIR)/%.o: %.cpp $(DEPEND)/%.d
	@mkdir -p $(DBGDIR)
	@mkdir -p $(dir $@)
	@$(MAKEDEPEND)
	$(CXX) $(CXXFLAGS) $(DBGCXXFLAGS) -c -o $@ $<

#
# Release rules
#
release: prep $(RELAPP)

$(RELAPP): $(RELOBJS)
	$(CXX) $^ $(LDFLAGS) $(RELLIB) -o $@

$(RELOBJS): $(RELDIR)/%.o: %.cpp $(DEPEND)/%.d
	@mkdir -p $(RELDIR)
	@mkdir -p $(dir $@)
	@$(MAKEDEPEND)
	$(CXX) $(CXXFLAGS) $(RELCXXFLAGS) -c -o $@ $<

#
# Other rules
#
prep:
	@mkdir -p $(DEPEND)

rebuild: clean all

clean:
	rm -f $(RELAPP) $(DBGAPP)
	rm -rf $(DEPEND) $(RELDIR) $(DBGDIR)

$(DEPEND)/%.d: ;
.PRECIOUS: $(DEPEND)/%.d

-include $(SRCS:%.cpp=$(DEPEND)/%.d)
