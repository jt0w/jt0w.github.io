{
  description = "Flake for Jt0w's website";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    chimera.url = "github:jt0w/chimera";
  };

  outputs = inputs @ {flake-parts, ...}:
    flake-parts.lib.mkFlake {inherit inputs;} {
      systems = ["x86_64-linux" "aarch64-linux" "aarch64-darwin" "x86_64-darwin"];

      perSystem = {
        self',
        pkgs,
        ...
      }:
      let
        name = "c";
        version = "0.1.0";
      in{
        devShells.default = pkgs.mkShell {
            inputsFrom = [
              self'.packages.default
            ];
            buildInputs = with pkgs;[
            python3
            ];
        };
        packages.default = pkgs.stdenv.mkDerivation {
          inherit version;
          pname = name;
          src = ./.;
          buildInputs = with pkgs; [gcc inputs.chimera.packages.${system}.default];
        };
      };
    };
}
