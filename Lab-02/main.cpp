#include<iostream>
#include<fstream>

using namespace std;

class LexicalAnalyzer{

    private:
    
    bool isAlpha(char ch){
        return (ch>='a' && ch<='z') || (ch<='A' && ch>='Z') || ch=='_';
    }

    bool isDigit(char ch){
        return ch>='0' && ch <= '9';
    }

    bool isSpace(char ch){
        return ch == ' ' || ch == '\r' || ch=='\n';
    }

    bool compare(const char *str1, const char *str2){

        while(*str1 && *str2 && *str1 == *str2){
            str1++;
            str2;
        }

        return *str1 == *str2;
    }

    bool checkKeyword(const char *buffer){

        const char *keywords[] = {
                    "alignas",      "alignof",     "and",         "and_eq",       asm
        auto         bitand      bitor       bool        break
        case         catch       char        char8_t     char16_t
        char32_t     class       compl       concept     const
        consteval    constexpr   constinit   continue    co_await
        co_return    co_yield    decltype    default     delete
        do           double      dynamic_cast else       enum
        explicit     export      extern      false       float
        for          friend      goto        if          inline
        int          long        mutable     namespace   new
        noexcept     not         not_eq      nullptr     operator
        or           or_eq       private     protected   public
        register     reinterpret_cast        requires    return
        short        signed      sizeof      static      static_assert
        static_cast  struct      switch      template    this
        thread_local throw       true        try         typedef
        typeid       typename    union       unsigned    using
        virtual      void        volatile    wchar_t     while
        xor          xor_eq

        };

        for(int i=0; i<(sizeof(keywords)/sizeof(keywords[0])); i++){

            if(compare(buffer,keywords[i])){
                return true;
            }

        }        
        return false;
    }

    public:

    void scanner(const char*filename){

        ifstream infile(filename);

        if(!infile || !infile.is_open()){
            return;
        }

        char ch;
        char buffer[100];
        int i;

        while(infile.get(ch)){

            if(isSpace(ch))continue;

            if(isAlpha(ch)){

                i = 0;
                buffer[i++] = ch;

                while(infile.get(ch) && (isAlpha(ch) || isDigit(ch))){
                    buffer[i++] = ch;
                }

                buffer[i] = '\0';
                infile.putback(ch);

                if(checkKeyword(buffer)){
                    cout << "Keyword: " << buffer << endl;
                }
                else{
                    cout << "Indentifyer: " << buffer << endl;
                }
            }
        }

        infile.close();
    }
};


int main(){

    LexicalAnalyzer().scanner("./example.txt");

    return 0;
}



