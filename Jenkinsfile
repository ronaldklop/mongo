pipeline {
	agent any

	stages {
		stage('Build') {
			steps {
				echo 'Building..'
				sh("""scons -j4 \
				    --use-system-zlib \
				    --use-system-pcre \
				    --use-system-snappy \
				    --libc++ \
				    --cxx-std=17 \
				    --runtime-hardening=on \
				    --disable-warnings-as-errors \
				    VERBOSE=on \
				    AR=/usr/local/bin/ar \
				    --use-sasl-client \
				    --ssl \
				    CC=cc \
				    "CCFLAGS=-O2 -pipe  -fstack-protector-strong -fno-strict-aliasing"  \
				    CPPPATH=/usr/local/include \
				    CXX=c++ \
				    "CXXFLAGS=-O2 -pipe -fstack-protector-strong -fno-strict-aliasing" \
				    LIBPATH=/usr/local/lib \
				    LINKFLAGS=-fstack-protector-strong \
				    PREFIX=/usr/local \
				    MONGO_VERSION=5.0.0-ronaldklop \
				    install-core""")
			}
		}
	}
}