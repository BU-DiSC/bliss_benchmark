// Generic interface to the disk.  Used by swap_space to store
// objects.

#ifndef BACKING_STORE_HPP
#define BACKING_STORE_HPP

#include <unistd.h>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <ext/stdio_filebuf.h>
#include <iostream>

class backing_store {
   public:
    virtual uint64_t allocate(size_t n) = 0;
    virtual void deallocate(uint64_t id) = 0;
    virtual std::iostream *get(uint64_t id) = 0;
    virtual void put(std::iostream *ios) = 0;
};

class one_file_per_object_backing_store : public backing_store {
   public:
    one_file_per_object_backing_store(std::string rt) : root(rt), nextid(1) {}
    uint64_t allocate(size_t n) {
        uint64_t id = nextid++;
        std::string filename = root + "/" + std::to_string(id);
        std::fstream dummy(filename, std::fstream::out);
        dummy.flush();
        assert(dummy.good());
        return id;
    }
    void deallocate(uint64_t id) {
        std::string filename = root + "/" + std::to_string(id);
        assert(unlink(filename.c_str()) == 0);
    }

    std::iostream *get(uint64_t id) {
        __gnu_cxx::stdio_filebuf<char> *fb = new __gnu_cxx::stdio_filebuf<char>;
        std::string filename = root + "/" + std::to_string(id);
        fb->open(filename, std::fstream::in | std::fstream::out);
        std::fstream *ios = new std::fstream;
        ios->std::ios::rdbuf(fb);
        ios->exceptions(std::fstream::badbit | std::fstream::failbit |
                        std::fstream::eofbit);
        assert(ios->good());

        return ios;
    }
    void put(std::iostream *ios) {
        ios->flush();
        __gnu_cxx::stdio_filebuf<char> *fb =
            (__gnu_cxx::stdio_filebuf<char> *)ios->rdbuf();
        fsync(fb->fd());
        delete ios;
        delete fb;
    }

   private:
    std::string root;
    uint64_t nextid;
};

#endif  // BACKING_STORE_HPP
