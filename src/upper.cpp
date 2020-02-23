
char upper(char c) noexcept
{
    return 
        static_cast<char>(
            static_cast<unsigned char>(
                c    
            ) & ~0x20
        );
}

