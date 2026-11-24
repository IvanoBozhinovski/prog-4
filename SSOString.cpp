#include <iostream>
#include <limits>
#include <cstring>
using namespace std;

class SSOString {
private:
    char* array;
    short sz;
    short cap;

    static int shortCap()
    {
        return sizeof(SSOString)-2;
    }

    char* raw()
    {
        return reinterpret_cast<char*>(this);
    }

    const char* raw() const
    {
        return reinterpret_cast<const char*>(this);
    }

    bool isShort() const
    {
        return raw()[0]&1;
    }

    char* data()
    {
        if(isShort())return raw()+1;
        return array;
    }

    const char* data() const
    {
        if(isShort())return raw()+1;
        return array;
    }

    void init(int n)
    {
        if(n>std::numeric_limits<short>::max()) cerr<<"The string is too long";
        if(n<=shortCap())
        {
            raw()[0]=static_cast<char>((n<<1)|1);
        }
        else
        {
            array=new char[n+1];
            sz=cap=static_cast<short>(n);
        }
        data()[n]='\0';
    }

    void release()
    {
        if(!isShort())delete[] array;
    }

public:
    SSOString(const char* str="")
    {
        int n=strlen(str);
        init(n);
        memcpy(data(),str,n); //strncpy zastanuva ako ima null terminator vo stringot
    }

    SSOString(const SSOString& other)
    {
        init(other.size());
        memcpy(data(),other.data(),other.size());
    }

    ~SSOString()
    {
        release();
    }

    SSOString& operator=(const SSOString& other)
    {
        if(this==&other)return *this;
        int n=other.size();
        if(other.isShort())
        {
            release();
            init(n);
            memcpy(data(),other.data(),n);
        }
        else
        {
            char* p=new char[n+1];
            memcpy(p,other.data(),n+1);
            release();
            array=p;
            sz=cap=static_cast<short>(n);
        }
        return *this;
    }

    int size() const
    {
        if(isShort()) return static_cast<char>(raw()[0])>>1;
        return sz;
    }

    int capacity() const
    {
        if(isShort()) return shortCap();
        return cap;
    }

    char& operator[](int i)
    {
        if(i>=0&&i<size())return data()[i];
        cerr<<"Out of range"<<i<<endl;
    }

    const char& operator[](int i) const
    {
        if(i>=0&&i<size())return data()[i];
        cerr<<"Out of range"<<i<<endl;
    }

    friend SSOString operator+(const SSOString& a,const SSOString& b)
    {
        SSOString res;
        res.init(a.size()+b.size());
        memcpy(res.data(),a.data(), a.size());
        memcpy(res.data()+a.size(),b.data(),b.size());
        return res;
    }

    friend ostream& operator<<(ostream& out,const SSOString& s)
    {
        return out<<s.data();
    }
};

int main() {
    SSOString a("Hello");
    SSOString b("World");

    SSOString c = a + b;
    SSOString d = c + c;

    std::cout << sizeof(SSOString) << '\n';

    std::cout << c << ' '
              << c.size() << ' '
              << c.capacity() << '\n';

    std::cout << d << ' '
              << d.size() << ' '
              << d.capacity() << '\n';

    d = a;
    d[0] = 'Y';

    std::cout << d << '\n';
}