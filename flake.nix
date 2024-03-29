{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "nixpkgs/nixos-unstable"; # primary nixpkgs
    flake-utils.url = "github:numtide/flake-utils";
    flake-compat = {
      url = "github:edolstra/flake-compat";
      flake = false;
    };
  };

  outputs = { self, nixpkgs, flake-utils, ... }@inputs:
    flake-utils.lib.eachSystem [ "x86_64-linux" "x86_64-darwin" ] (system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
      {
        devShell = pkgs.mkShell {
          nativeBuildInputs = with pkgs; [
            clang-tools
            clang-analyzer
            cppcheck
            flex
            bison
            # used by bison to produce html file
            libxslt
            # generate compile_commands.json
            python310Packages.compiledb
          ];
          buildInputs = with pkgs; [
            gcc
            criterion
            gmp
          ];
        };
      }
    );
}
