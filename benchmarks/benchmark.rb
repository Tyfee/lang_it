book_file       = "./o_pequeno_principe.txt"
dictionary_file = "./dicionario_pt_usp.txt"
book_output     = "./result.txt"
dict_output     = "./translated_result.txt"
english_dict    = "./dictionary_en_david47k.txt"
translate_exe   = "../translate_line"

book_lines_translated = 0
dictionary_lines_translated = 0
true_positives = 0

# Load English dictionary into a Set for fast lookup
english_words = Set.new(File.readlines(english_dict).map(&:strip).reject(&:empty?))

# --- Process book ---
File.open(book_output, "w") do |out|
  IO.popen(translate_exe, "r+") do |io|
    puts "Translating book..."
    File.foreach(book_file).with_index(1) do |line, line_index|
      line.strip!
      next if line.empty?

      io.puts line
      io.flush

      translation = io.gets&.strip
      next unless translation && !translation.empty? && translation != line

      out.puts translation
      book_lines_translated += 1

      puts "Processed book line #{line_index}" if line_index % 50 == 0
    end
  end
end

# --- Process dictionary ---
File.open(dict_output, "w") do |out|
  IO.popen(translate_exe, "r+") do |io|
    puts "Translating dictionary..."
    bad_tokens = ["e", "es", "ee"]

    File.foreach(dictionary_file).with_index(1) do |line, line_index|
      line.strip!
      next if line.empty?

      io.puts line
      io.flush

      translation = io.gets&.strip
      next unless translation && !translation.empty? && translation != line
      next if bad_tokens.include?(translation)
      next unless translation.match?(/[A-Za-zÀ-ÖØ-öø-ÿ]/)

      # Write all translations to output
      out.puts translation
      dictionary_lines_translated += 1

      # Check true positives
      true_positives += 1 if english_words.include?(translation.downcase)

      puts "Processed dictionary line #{line_index}" if line_index % 50 == 0
    end
  end
end

puts "Book benchmark completed! #{book_lines_translated} lines translated to #{book_output}"
puts "Dictionary benchmark completed! #{dictionary_lines_translated} lines translated to #{dict_output}"
puts "Number of dictionary translations that exist in English dictionary: #{true_positives}"
