


template<typename T, typename length>
std::string Mesh::base64_encode(const std::vector<T> &vals){
    // Base 64 encoding table. Each Letter represents a number between 0-63.
    std::string b64_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

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

    // Convert bit stream to base 64 values by taking chunks of data 24 bits wide
    // and converting a quad of 6-bit data to base64.
    std::string result = "";
    result.reserve( ceil(bit_string.size() / 6) );
    for (unsigned int bit_index = 0; bit_index < bit_string.size(); bit_index += 24){

        // Get next data chunk of 24 bits
        std::string block_bit_value = bit_string.substr(bit_index, 24);

        // Break the chunk into 6-bit increments for converting to base64
        for (unsigned int i = 0; i < 24; i+=6){

            // Leave this loop if we've run out of data to convert. This will
            // require the data to be padded with '=' characters
            if (block_bit_value.size() < i)
                break;

            // Gather 6-bit piece of data chunk and pad with '0' if remaining
            // data is less than 6 bits in size
            std::string chunk_bit_value = block_bit_value.substr(i, 6);
            chunk_bit_value.append(6-chunk_bit_value.size(), '0');

            // Convert the 6-bit data to an integer value for table lookup
            unsigned int table_index = std::bitset<6>(chunk_bit_value).to_ulong();
            result += b64_table[table_index];
        }
    }

    // Pad the final result with '=' characters if the data stream did not completely
    // fill the data chunks of 24 bits.
    result.append( (result.size()-1) % 3, '=');
    return result;
}
