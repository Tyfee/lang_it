input_file = "./o_pequeno_principe.txt"
output_file = "./result.txt"

line_count = 0

File.open(output_file, "w") do |out|
  IO.popen("../translate_line", "r+") do |io|
    File.foreach(input_file) do |line|
      line_count += 1

      io.puts line
      io.flush

      translation = io.gets
      out.puts translation if translation

      puts "Processed line #{line_count}" if line_count % 50 == 0
    end
  end
end

puts "Translation complete! Total lines: #{line_count}"
