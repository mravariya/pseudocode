# Homebrew formula (same repo tap). Install:
#   brew tap mravariya/pseudocode https://github.com/mravariya/pseudocode
#   brew install --HEAD mravariya/pseudocode/pseudocode
#
# Or from a local clone:
#   brew install --HEAD ./Formula/pseudocode.rb
class Pseudocode < Formula
  desc "Interpreter for Cambridge International-style pseudocode (CS9618, CS0478, IT9626)"
  homepage "https://mravariya.github.io/pseudocode/"
  license "MIT"
  head "https://github.com/mravariya/pseudocode.git", branch: "main"

  depends_on "cmake" => :build
  depends_on "readline"

  def install
    system "cmake", "-S", ".", "-B", "build", "-DPC_EMBED_INSTALL_STDLIB=ON", *std_cmake_args
    system "cmake", "--build", "build"
    system "cmake", "--install", "build"
  end

  test do
    assert_match(/pseudocode/i, shell_output("#{bin}/pseudocode version"))
    assert_match(/pkg/i, shell_output("#{bin}/pkg version"))
  end
end
