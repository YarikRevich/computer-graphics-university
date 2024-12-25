#include "encode.hpp"
#include <iostream>

Encode::Encode(args::ArgumentParser *argumentParser)
{
    this->command = new args::Command(*argumentParser, "encode", "Encodes given media");
    args::Group *group = new args::Group(*command, "");
    this->from = new args::ValueFlag<std::string>(*group, "path", "Path to the source media", {"from"});
    this->type = new args::ValueFlag<std::string>(*group, "bmp|jpg|jpeg|png", "Type of the source media", {"type"});
    this->conversion = new args::ValueFlag<std::string>(*group, "native_colorful|native_bw|palette_colorful|palette_bw", "Type of the media conversion. Available only with 7 bit mode.", {"conversion"});
    this->bit = new args::ValueFlag<std::string>(*group, "24|16|15|7", "Amount of bits used for the media conversion", {"bit"});
    this->model = new args::ValueFlag<std::string>(*group, "rgb|yuv|yiq|ycbcr|hsl", "Type of color model to be used for media conversion", {"model"});
    this->losslessCompression = new args::ValueFlag<std::string>(*group, "byterun|rle|lzw|lz77", "Type of lossless compression to be used for media conversion", {"losslessCompression"});
    this->lossyCompression = new args::ValueFlag<std::string>(*group, "dct", "Type of lossy compression to be used for media conversion, can be combined with losless compression", {"lossyCompression"});
    this->sampling = new args::ValueFlag<std::string>(*group, "four_one_one", "Type of sampling to be applied", {"sampling"});
    this->filter = new args::ValueFlag<std::string>(*group, "differential|line_difference|average|paeth", "Type of filter to be used for media conversion", {"filter"});
    this->dithering = new args::Flag(*group, "true|false(default)", "Enables dithering for the output stream", {"dithering"});
    this->to = new args::ValueFlag<std::string>(*group, "path", "Path to the output media", {"to"});
}

bool Encode::isCalled()
{
    return command->Matched();
}

int Encode::handle()
{
    if (!from->Matched())
    {
        Validator::throwValueFlagRequiredException("from");
        return EXIT_FAILURE;
    }

    if (!type->Matched())
    {
        Validator::throwValueFlagRequiredException("type");
        return EXIT_FAILURE;
    }

    if (!conversion->Matched())
    {
        Validator::throwValueFlagRequiredException("conversion");
        return EXIT_FAILURE;
    }

    if (!bit->Matched())
    {
        Validator::throwValueFlagRequiredException("bit");
        return EXIT_FAILURE;
    }

    if (!model->Matched())
    {
        Validator::throwValueFlagRequiredException("model");
        return EXIT_FAILURE;
    }

    if (!to->Matched())
    {
        Validator::throwValueFlagRequiredException("to");
        return EXIT_FAILURE;
    }

    IO::CONVERSION_TYPES conversionType = IO::getConversionType(conversion->Get());

    IO::BIT_TYPES bitType = IO::getBitType(bit->Get());
    if (bitType == IO::BIT_TYPES::NONE) {
        Validator::throwValueFlagInvalidException("bit");
        return EXIT_FAILURE;
    }

    if (conversionType != IO::CONVERSION_TYPES::NONE && bitType != IO::BIT_TYPES::SEVEN) {
        Validator::throwValueFlagInvalidException("'bit' flag should be equal to '7' to use conversion");
        return EXIT_FAILURE;
    }

    IO::MODEL_TYPES modelType = IO::getModelType(model->Get());
    if (modelType == IO::MODEL_TYPES::NONE) {
        Validator::throwValueFlagInvalidException("model");
        return EXIT_FAILURE;
    }

    SDL_Surface *input;

    switch (IO::getFileType(type->Get()))
    {
    case IO::FILE_TYPES::JPG:
        input = IO::readFileJPEG(from->Get());
        break;
    case IO::FILE_TYPES::PNG:
        input = IO::readFilePNG(from->Get());
        break;
    case IO::FILE_TYPES::BMP:
        input = IO::readFileBMP(from->Get());
        break;
    default:
        Validator::throwValueFlagInvalidException("type");
        return EXIT_FAILURE;
    }

    if (input == NULL)
    {
        Validator::throwValueFlagInvalidException("from");
        return EXIT_FAILURE;
    }

    std::ofstream outputStream(to->Get(), std::ios_base::out | std::ios_base::binary);
    if (!outputStream.is_open())
    {
        Validator::throwValueFlagInvalidException("from");
        return EXIT_FAILURE;
    }

    int result = Pipeline::handleEncode(
        input, 
        conversionType, 
        bitType, 
        modelType, 
        IO::getLosslessCompressionType(losslessCompression->Get()),
        IO::getLossyCompressionType(lossyCompression->Get()),
        IO::getSamplingType(sampling->Get()),
        IO::getFilterType(filter->Get()),
        dithering->Get(),
        outputStream);

    outputStream.close();

    if (result != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}