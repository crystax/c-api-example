HOST_OS := $(shell uname -s | tr '[A-Z]' '[a-z]')
HOST_ARCH := $(shell uname -m)

ifneq (,$(strip $(NDK)))
so = so
soname = soname
else
ifeq (linux,$(HOST_OS))
so = so
soname = soname
ldlp = LD_LIBRARY_PATH
else
ifeq (darwin,$(HOST_OS))
so = dylib
soname = install_name
ldlp = DYLD_LIBRARY_PATH
else
$(error Unsupported host OS: $(HOST_OS))
endif
endif
endif

ifeq (,$(strip $(NDK)))
BINDIR = bin/$(HOST_OS)
OBJDIR = obj/$(HOST_OS)
else
BINDIR = bin/android
OBJDIR = obj/android
endif

LIBV1   = $(BINDIR)/libv1/libv.$(so)
LIBV2   = $(BINDIR)/libv2/libv.$(so)
LIBVCXX = $(BINDIR)/libvcxx.a

APP = $(BINDIR)/app
APPLIB = $(BINDIR)/libapp.so

LIBV1_SOURCES   = src/version-impl.cpp src/geo-impl.cpp src/md-impl.cpp
LIBV2_SOURCES   = $(LIBV1_SOURCES) src/map-impl.cpp
LIBVCXX_SOURCES = src/version.cpp src/geo.cpp src/md.cpp src/map.cpp

APPLIB_SOURCES = test/applib.cpp
APP_SOURCES    = test/app.cpp

ifeq (,$(strip $(NDK)))
CXX := g++
AR  := ar
else
CXX := $(NDK)/toolchains/arm-linux-androideabi-4.9/prebuilt/$(HOST_OS)-$(HOST_ARCH)/bin/arm-linux-androideabi-g++
CXX += -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -mthumb
CXX += --sysroot=$(NDK)/platforms/android-9/arch-arm
CXX += -I$(NDK)/sources/cxx-stl/gnu-libstdc++/4.9/include
CXX += -I$(NDK)/sources/cxx-stl/gnu-libstdc++/4.9/libs/armeabi-v7a/include

AR  := $(NDK)/toolchains/arm-linux-androideabi-4.9/prebuilt/$(HOST_OS)-$(HOST_ARCH)/bin/arm-linux-androideabi-ar
endif

CXXFLAGS := -O0 -g
CXXFLAGS += -fPIC
CXXFLAGS += -Iinclude

LDFLAGS :=
ifneq (,$(strip $(NDK)))
LDFLAGS += -L$(NDK)/sources/cxx-stl/gnu-libstdc++/4.9/libs/armeabi-v7a
LDFLAGS += -L$(NDK)/sources/crystax/libs/armeabi-v7a
LDFLAGS += -lgnustl_shared
else
ifeq (linux,$(HOST_OS))
LDFLAGS += -ldl
endif
endif

DEVPATH := /data/local/tmp/cxxapi

LIBV1_OBJECTS   := $(patsubst %.cpp,$(OBJDIR)/libv1/%.o,$(LIBV1_SOURCES))
LIBV2_OBJECTS   := $(patsubst %.cpp,$(OBJDIR)/libv2/%.o,$(LIBV2_SOURCES))
LIBVCXX_OBJECTS := $(patsubst %.cpp,$(OBJDIR)/libvcxx/%.o,$(LIBVCXX_SOURCES))
APPLIB_OBJECTS  := $(patsubst %.cpp,$(OBJDIR)/applib/%.o,$(APPLIB_SOURCES))
APP_OBJECTS     := $(patsubst %.cpp,$(OBJDIR)/app/%.o,$(APP_SOURCES))

.PHONY: run
ifeq (,$(strip $(NDK)))
run: all
	@echo "=== Run with libv1:"
	$(ldlp)=$(dir $(APPLIB)):$(dir $(LIBV1)) ./$(APP)
	@echo "=== Run with libv2:"
	$(ldlp)=$(dir $(APPLIB)):$(dir $(LIBV2)) ./$(APP)
else
run: all
	@echo "=== Deploy binaries to device"
	adb shell 'test -e $(DEVPATH) && rm -r $(DEVPATH)'
	adb shell 'mkdir -p $(DEVPATH)'
	adb shell 'mkdir -p $(DEVPATH)/libv1'
	adb shell 'mkdir -p $(DEVPATH)/libv2'
	adb push $(NDK)/sources/crystax/libs/armeabi-v7a/libcrystax.so $(DEVPATH)/
	adb push $(NDK)/sources/cxx-stl/gnu-libstdc++/4.9/libs/armeabi/libgnustl_shared.so $(DEVPATH)/
	adb push $(LIBV1) $(DEVPATH)/libv1/
	adb push $(LIBV2) $(DEVPATH)/libv2/
	adb push $(APPLIB) $(DEVPATH)/
	adb push $(APP) $(DEVPATH)/
	adb shell 'chmod 0700 $(DEVPATH)/$(notdir $(APP))'
	@echo "=== Run with libv1:"
	adb shell 'cd $(DEVPATH) && LD_LIBRARY_PATH=$(DEVPATH):$(DEVPATH)/libv1 ./$(notdir $(APP))'
	@echo "=== Run with libv2:"
	adb shell 'cd $(DEVPATH) && LD_LIBRARY_PATH=$(DEVPATH):$(DEVPATH)/libv2 ./$(notdir $(APP))'
endif

.PHONY: all
all: $(APP) $(LIBV1) $(LIBV2)

.PHONY:
clean:
	-rm -Rf obj bin

$(BINDIR)/libv1/:
	mkdir -p $@

$(BINDIR)/libv2/:
	mkdir -p $@

$(BINDIR):
	mkdir -p $@

$(OBJDIR)/libv1/src:
	mkdir -p $@

$(OBJDIR)/libv2/src:
	mkdir -p $@

$(OBJDIR)/libvcxx/:
	mkdir -p $@

$(OBJDIR)/libvcxx/src:
	mkdir -p $@

$(OBJDIR)/app/test:
	mkdir -p $@

$(OBJDIR)/applib/test:
	mkdir -p $@

$(APP): $(APP_OBJECTS) $(APPLIB) $(LIBVCXX) $(LIBV1) $(LIBV2) $(MAKEFILE_LIST) | $(dir $(APP))
	$(CXX) -o $@ $(APP_OBJECTS) -fPIE -pie -L$(dir $(APPLIB)) -L$(dir $(LIBV2)) -lapp -lv $(LDFLAGS)

$(APPLIB): $(APPLIB_OBJECTS) $(LIBVCXX) $(LIBV1) $(LIBV2) $(MAKEFILE_LIST) | $(dir $(APPLIB))
	$(CXX) -shared -fPIC -o $@ $(APPLIB_OBJECTS) -L$(dir $(LIBVCXX)) -L$(dir $(LIBV2)) -lvcxx -lv $(LDFLAGS)

$(LIBV1): $(LIBV1_OBJECTS) $(MAKEFILE_LIST) | $(dir $(LIBV1))
	$(CXX) -shared -Wl,-$(soname),libv.$(so) -fPIC -o $@ $(LIBV1_OBJECTS) $(LDFLAGS)

$(LIBV2): $(LIBV2_OBJECTS) $(MAKEFILE_LIST) | $(dir $(LIBV2))
	$(CXX) -shared -Wl,-$(soname),libv.$(so) -fPIC -o $@ $(LIBV2_OBJECTS) $(LDFLAGS)

$(LIBVCXX): $(LIBVCXX_OBJECTS) $(LIBV1) $(LIBV2) $(MAKEFILE_LIST) | $(dir $(LIBVCXX))
	$(AR) rcs $@ $(LIBVCXX_OBJECTS)

$(LIBV1_OBJECTS): $(LIBV1_SOURCES)
$(LIBV2_OBJECTS): $(LIBV2_SOURCES)

$(OBJDIR)/libv1/%.o: %.cpp $(MAKEFILE_LIST) | $(OBJDIR)/libv1/src
	$(CXX) $(CXXFLAGS) -DAPILEVEL=1 -c -o $@ $<

$(OBJDIR)/libv2/%.o: %.cpp $(MAKEFILE_LIST) | $(OBJDIR)/libv2/src
	$(CXX) $(CXXFLAGS) -DAPILEVEL=2 -c -o $@ $<

$(OBJDIR)/libvcxx/%.o: %.cpp $(MAKEFILE_LIST) | $(OBJDIR)/libvcxx/src
	$(CXX) $(CXXFLAGS) -DAPILEVEL=2 -DLIBV='"libv.$(so)"' -c -o $@ $<

$(OBJDIR)/app/%.o: %.cpp $(MAKEFILE_LIST) | $(OBJDIR)/app/test $(APPLIB)
	$(CXX) $(CXXFLAGS) -DAPILEVEL=2 -c -o $@ $<

$(OBJDIR)/applib/%.o: %.cpp $(MAKEFILE_LIST) | $(OBJDIR)/applib/test $(LIBVCXX) $(LIBV1) $(LIBV2)
	$(CXX) $(CXXFLAGS) -DAPILEVEL=2 -c -o $@ $<
