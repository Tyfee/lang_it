require 'set'

book_file       = "./corpora/o_pequeno_principe.txt"
dictionary_file = "./dictionaries/dicionario_pt_usp.txt"
book_output     = "./results/book_result_pt_en.txt"
dict_all_output = "./results/dict_result_pt_en_all.txt"
dict_pos_output = "./results/dict_result_pt_en_positive.txt"
english_dict    = "./dictionaries/dictionary_en_david47k.txt"
translate_exe   = "./translate_line"

book_lines_translated = 0
dictionary_lines_translated_all = 0
dictionary_lines_translated_pos = 0
true_positives = 0

def safe_strip(line)
  return "" if line.nil?
  line.encode("UTF-8", invalid: :replace, undef: :replace, replace: "").strip
end

# Load English dictionary
english_words = Set.new(
  File.readlines(english_dict, encoding: "UTF-8", invalid: :replace, undef: :replace)
      .map { |l| safe_strip(l) }
      .reject(&:empty?)
)

# --- Process book ---
File.open(book_output, "w", encoding: "UTF-8") do |out|
  IO.popen(translate_exe, "r+") do |io|
    io.set_encoding("UTF-8", "UTF-8")
    puts "Translating book..."
    File.foreach(book_file, encoding: "UTF-8", invalid: :replace, undef: :replace).with_index(1) do |line, line_index|
      line = safe_strip(line)
      next if line.empty?

      io.puts line
      io.flush

      translation = safe_strip(io.gets)
      next if translation.empty? || translation == line

      out.puts translation
      book_lines_translated += 1

      puts "Processed book line #{line_index}" if line_index % 50 == 0
    end
  end
end

# --- Process dictionary (all + true positives) ---
File.open(dict_all_output, "w", encoding: "UTF-8") do |all_out|
  File.open(dict_pos_output, "w", encoding: "UTF-8") do |pos_out|
    IO.popen(translate_exe, "r+") do |io|
      io.set_encoding("UTF-8", "UTF-8")
      puts "Translating dictionary..."
      bad_tokens = ["e", "es", "ee", "ing"]

      File.foreach(dictionary_file, encoding: "UTF-8", invalid: :replace, undef: :replace).with_index(1) do |line, line_index|
        line = safe_strip(line)
        next if line.empty?

        io.puts line
        io.flush

        translation = safe_strip(io.gets)
        next if translation.empty? || translation == line
        next if bad_tokens.include?(translation)
        next unless translation.match?(/[A-Za-zÀ-ÖØ-öø-ÿ]/)

        # Write all translations
        all_out.puts translation
        dictionary_lines_translated_all += 1

        # Write only true positives
        if english_words.include?(translation.downcase)
          pos_out.puts translation
          dictionary_lines_translated_pos += 1
          true_positives += 1
        end

        puts "Processed dictionary line #{line_index}" if line_index % 50 == 0
      end
    end
  end
end

puts "Book benchmark completed! #{book_lines_translated} lines translated to #{book_output}"
puts "Dictionary benchmark completed! #{dictionary_lines_translated_all} lines translated to #{dict_all_output}"
puts "Number of dictionary translations that exist in English dictionary: #{dictionary_lines_translated_pos} (saved to #{dict_pos_output})"
