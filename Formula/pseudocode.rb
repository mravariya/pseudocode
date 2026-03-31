# Homebrew: same-repo tap (stable + optional HEAD).
#
# Default install (stable tarball, like python bottles from source):
#   brew tap mravariya/pseudocode https://github.com/mravariya/pseudocode
#   brew install mravariya/pseudocode/pseudocode
#
# Latest main without waiting for a tag:
#   brew install --HEAD mravariya/pseudocode/pseudocode
#
# Or from a clone:
#   brew install ./Formula/pseudocode.rb
class Pseudocode < Formula
  desc "Interpreter for Cambridge International-style pseudocode (CS9618, CS0478, IT9626)"
  homepage "https://mravariya.github.io/pseudocode/"
  license "MIT"
  url "https://github.com/mravariya/pseudocode/archive/refs/tags/v1.0.0.tar.gz"
  sha256 "492920be9b6f76f046c652cbabb5d72c9abe79b913daf1e70ddfb1d4d6764325"
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
