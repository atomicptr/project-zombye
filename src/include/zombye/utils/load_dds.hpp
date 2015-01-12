#ifndef __ZOMBYE_LOAD_DDS_HPP__
#define __ZOMBYE_LOAD_DDS_HPP__

#include <sstream>

#include <gli/gli.hpp>

namespace gli {
    inline storage load_dds(const std::vector<char>& content) {
        std::stringstream FileIn(std::string(content.data(), content.size()));
        assert(!FileIn.fail());

        if(FileIn.fail())
            return storage();

        detail::ddsHeader HeaderDesc;
        detail::ddsHeader10 HeaderDesc10;
        char Magic[4];

        //* Read magic number and check if valid .dds file
        FileIn.read((char*)&Magic, sizeof(Magic));

        assert(strncmp(Magic, "DDS ", 4) == 0);

        // Get the surface descriptor
        FileIn.read((char*)&HeaderDesc, sizeof(HeaderDesc));
        if(HeaderDesc.format.flags & detail::DDPF_FOURCC && HeaderDesc.format.fourCC == detail::D3DFMT_DX10)
            FileIn.read((char*)&HeaderDesc10, sizeof(HeaderDesc10));

        gli::format Format(gli::FORMAT_NULL);
        if(HeaderDesc.format.fourCC == detail::D3DFMT_DX10)
            Format = detail::format_dds2gli_cast(HeaderDesc10.Format);
        else if(HeaderDesc.format.flags & detail::DDPF_FOURCC)
            Format = detail::format_fourcc2gli_cast(HeaderDesc.format.flags, HeaderDesc.format.fourCC);
        else if(HeaderDesc.format.flags & detail::DDPF_RGB)
        {
            switch(HeaderDesc.format.bpp)
            {
            case 8:
                Format = R8_UNORM;
                break;
            case 16:
                Format = RG8_UNORM;
                break;
            case 24:
                Format = RGB8_UNORM;
                break;
            case 32:
                Format = RGBA8_UNORM;
                break;
            }
        }
        else
            assert(0);

        std::streamoff Curr = FileIn.tellg();
        FileIn.seekg(0, std::ios_base::end);
        std::streamoff End = FileIn.tellg();
        FileIn.seekg(Curr, std::ios_base::beg);

        storage::size_type const MipMapCount = (HeaderDesc.flags & detail::DDSD_MIPMAPCOUNT) ?
            HeaderDesc.mipMapLevels : 1;

        storage::size_type FaceCount(1);
        if(HeaderDesc.cubemapFlags & detail::DDSCAPS2_CUBEMAP)
            FaceCount = int(glm::bitCount(HeaderDesc.cubemapFlags & detail::DDSCAPS2_CUBEMAP_ALLFACES));

        storage::size_type DepthCount = 1;
        if(HeaderDesc.cubemapFlags & detail::DDSCAPS2_VOLUME)
                DepthCount = HeaderDesc.depth;

        storage Storage(
            HeaderDesc10.arraySize,
            FaceCount,
            MipMapCount,
            Format,
            storage::dimensions_type(HeaderDesc.width, HeaderDesc.height, DepthCount));

        FileIn.read((char*)Storage.data(), std::size_t(End - Curr));

        return Storage;
    }
}

#endif
