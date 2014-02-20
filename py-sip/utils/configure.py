import os
import sipconfig

# The name of the SIP build file generated by SIP and used by the build
# system.
build_file = "PyYmUtils.sbf"

# Get the sip configuration information.
config = sipconfig.Configuration()

# Get the extra SIP flags needed by the imported PyQt modules.  Note that
# this normally only includes those flags (-x and -t) that relate to SIP's
# versioning system.
sip_flags = ""

# Run SIP to generate the code.  Note that we tell SIP where to find the qt
# module's specification files using the -I flag.
sipsrc="YmUtils_mod.sip"
os.system(" ".join([config.sip_bin, "-c", ".", "-b", build_file,
                    "-I", config.default_sip_dir,
                    "-s", ".cc",
                    sip_flags, sipsrc]))

# We are going to install the SIP specification file for this module and
# its configuration module.
installs = []

my_sip_dir = "/home/matsunaga/share/py-sip"

installs.append(["YmUtils_mod.sip", os.path.join(my_sip_dir, "OthCore")])
installs.append(["Ban.sip", os.path.join(my_sip_dir, "OthCore")])

installs.append(["YmUtilsConfig.py", config.default_mod_dir])

# Create the Makefile.  The QtGuiModuleMakefile class provided by the
# pyqtconfig module takes care of all the extra preprocessor, compiler and
# linker flags needed by the Qt library.
makefile = sipconfig.SIPModuleMakefile(
    configuration=config,
    build_file=build_file,
    installs=installs
)

# Add the library we are wrapping.  The name doesn't include any platform
# specific prefixes or extensions (e.g. the "lib" prefix on UNIX, or the
# ".dll" extension on Windows).
makefile.extra_libs = ["ym_utils"]
makefile.extra_include_dirs = ["/home/yusuke/include"]
makefile.extra_lib_dirs = ["/home/yusuke/lib"]

# Generate the Makefile itself.
makefile.generate()

# Now we create the configuration module.  This is done by merging a Python
# dictionary (whose values are normally determined dynamically) with a
# (static) template.
content = {
    # Publish where the SIP specifications for this module will be
    # installed.
    "YmUtils_sip_dir":    my_sip_dir,

    # Publish the set of SIP flags needed by this module.  As these are the
    # same flags needed by the qt module we could leave it out, but this
    # allows us to change the flags at a later date without breaking
    # scripts that import the configuration module.
    "YmUtils_sip_flags":  ""
}

# This creates the helloconfig.py module from the helloconfig.py.in
# template and the dictionary.
sipconfig.create_config_module("YmUtilsConfig.py", "YmUtilsConfig.py.in", content)
