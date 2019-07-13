
template<typename T, typename length>
std::string MeshWriter::vtkDataToBinary(const std::vector<T> &vals){
    unsigned int nBytesData = vals.size() * sizeof(length);
    std::string converted_header = base64_encode(getBitVector<unsigned int,int32_t>({nBytesData}));
    std::string converted_data = base64_encode(getBitVector<T,length>(vals));
    return converted_header + converted_data;
}

template<typename T, typename length>
std::string MeshWriter::vtkDataToCompressedBinary(const std::vector<T> &vals){
    return "";
}

template<typename T, typename length>
std::vector<uint8_t> MeshWriter::getBitVector(const std::vector<T> &vals){
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
    std::vector<uint8_t> byte_vector;
    byte_vector.reserve(sizeof(length) * vals.size());
    for (const auto item : vals){
        data.input = item;
        std::string bit_string = std::bitset<n_bits_output_type>(data.output).to_string();
        // Convert bit string to array of 8-bit integers and append to byte vector
        for (unsigned int index = 0; index < bit_string.size(); index += 8){
            byte_vector.push_back( std::bitset<8>(bit_string.substr(index, 8)).to_ulong() );
        }
    }

    return byte_vector;
}