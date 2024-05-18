openssl = {
    source = path.join(dependencies.basePath, "openssl"),
}

function openssl.import()
    links { "ssl", "crypto" }
    openssl.includes()
end

function openssl.includes()
    includedirs {
        path.join(openssl.source, "include"),
    }

    defines {
        "OPENSSL_NO_SSL2",
        "OPENSSL_NO_SSL3",
        "OPENSSL_NO_COMP",
        "OPENSSL_NO_ASM",
        "OPENSSL_NO_JPAKE",
        "OPENSSL_NO_KRB5",
        "OPENSSL_NO_DYNAMIC_ENGINE",
    }
end

function openssl.project()
    project "openssl"
        language "C"

        openssl.includes()

        files {
            path.join(openssl.source, "crypto/**.c"),
            path.join(openssl.source, "crypto/**.h"),
            path.join(openssl.source, "ssl/**.c"),
            path.join(openssl.source, "ssl/**.h"),
        }

        removefiles {
            path.join(openssl.source, "crypto/bn/asm/*.asm"),
            path.join(openssl.source, "crypto/ec/asm/*.asm"),
            path.join(openssl.source, "crypto/evp/asm/*.asm"),
            path.join(openssl.source, "crypto/modes/asm/*.asm"),
            path.join(openssl.source, "crypto/poly1305/asm/*.asm"),
            path.join(openssl.source, "crypto/sha/asm/*.asm"),
            path.join(openssl.source, "crypto/x86_64cpuid/*.asm"),
        }

        defines {
            "OPENSSL_BUILDING_OPENSSL",
        }

        filter "toolset:msc*"
            defines {
                "WIN32_LEAN_AND_MEAN",
                "VC_EXTRALEAN",
                "_CRT_SECURE_NO_WARNINGS",
                "OPENSSL_SYS_WINDOWS",
                "OPENSSL_MSVC_STATIC",
                "L_ENDIAN",
            }
            links {
                "Crypt32.lib",
                "WS2_32.lib",
                "Advapi32.lib",
            }

        filter "toolset:not msc*"
            defines {
                "OPENSSL_SYS_UNIX",
                "OPENSSL_PIC",
            }

        filter {}

        warnings "Off"
        kind "StaticLib"
end

table.insert(dependencies, openssl)
