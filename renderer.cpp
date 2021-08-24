global_variable std::vector<texture_unit> TextureCache;

internal int
CheckTextureCache(char *TexturePath)
{
    for(int i = 0; i < TextureCache.size(); ++i)
    {
        if(strcmp(TexturePath, TextureCache[i].Path) == 0)
        {
            return i;
        }
    }
    
    return -1;
}
