import qbs

QtApplication {
    cpp.cxxLanguageVersion: "c++11"

    cpp.dynamicLibraries: [ "sodium", "argon2" ]

    cpp.includePaths: [ "External", "Internal" ]

    consoleApplication: true
    files: [
        "Internal/Encryption.cpp",
        "Internal/Encryption.h",
        "Internal/Hashing.cpp",
        "Internal/Hashing.h",
        "Internal/Header.cpp",
        "Internal/Header.h",
        "Internal/Msec.cpp",
        "Internal/Msec.h",
        "Internal/Utils.cpp",
        "Internal/Utils.h",
        "External/Command.cpp",
        "External/Command.h",
        "External/Global.cpp",
        "External/Global.h",
        "External/Args.cpp",
        "External/Args.h",
        "main.cpp",
    ]

    Group {     // Properties for the produced executable
        fileTagsFilter: "application"
        qbs.install: true
        qbs.installDir: "bin"
    }
}
