with import <nixpkgs> {};

let
  unstable = import
    (fetchTarball
      https://github.com/NixOS/nixpkgs-channels/archive/nixos-unstable.tar.gz) {};
in
stdenv.mkDerivation {
  name = "crnsimul";
  src = ./.;

  buildInputs = [ cmake clang boost flex bison gnuplot gtest glew unstable.glfw freetype glm fontconfig ];

  buildPhase = "cmake . && make && ./bin/tests";

  installPhase = ''
    mkdir -p $out/bin
    cp bin/crnsimul $out/bin/
  '';
}
