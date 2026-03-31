# Template for a Homebrew/homebrew-core pull request (copy into Formula/pseudocode.rb there).
# After merge: brew install pseudocode
# See docs/package-managers.md for audit, fork, and PR steps.
#
# Update url + sha256 to match a release tag on github.com/mravariya/pseudocode
class Pseudocode < Formula
  desc "Interpreter for Cambridge International-style pseudocode (CS9618, CS0478, IT9626)"
  homepage "https://mravariya.github.io/pseudocode/"
  url "https://github.com/mravariya/pseudocode/archive/refs/tags/v1.0.0.tar.gz"
  sha256 "HOMEBREW_SHA256_PLACEHOLDER"
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
