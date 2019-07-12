
template<typename T, typename length>
std::string MeshWriter::vtkDataToBinary(const std::vector<T> &vals){
    unsigned int nBytesData = vals.size() * sizeof(length);
    std::string converted_header = base64_encode(getBitStream<unsigned int,int32_t>({nBytesData}));
    std::string converted_data = base64_encode(getBitStream<T,length>(vals));
    return converted_header + converted_data;
}

template<typename T, typename length>
std::string MeshWriter::getBitStream(const std::vector<T> &vals){
    // Used to convert the vector of input values to binary representations.
    // inputs of type T can be (unsigned int, float, etc), while outputs of
    // type length are fixed width integer types (int32_t, uint8_t, etc.).
    union {
        T input;
        length output;
    } data;
    
    // Convert vector of input data into a bit string of length 
    // {n_bytes_output_type} * 8 * {n_inputs} -> n_total_bits
    const std::size_t n_bits_output_type = sizeof(length) * 8;
    std::string bit_string = "";
    bit_string.reserve(n_bits_output_type * vals.size());
    for (const auto item : vals){
        data.input = item;
        bit_string += std::bitset<n_bits_output_type>(data.output).to_string();
    }

    return bit_string;
}
