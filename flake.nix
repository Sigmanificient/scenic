{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs = {
    self,
    nixpkgs,
  }: let
    inherit (nixpkgs) lib;

    applySystems = lib.genAttrs [
      "x86_64-linux"
      "aarch64-linux"
    ];
    eachSystem = f:
      applySystems (system:
        f {
          inherit system;
          pkgs = nixpkgs.legacyPackages.${system};
        });
  in {
    formatter = eachSystem ({pkgs, ...}: pkgs.alejandra);

    devShells = eachSystem ({
      pkgs,
      system,
    }: {
      default = pkgs.mkShell {
        env = {
          CC = pkgs.stdenv.cc;
          MAKEFLAGS = "-j";
          USE_LOCAL_DIRS = true;
        };

        packages = with pkgs; [
          hl-log-viewer
          compiledb
          gcovr
          pkg-config
          (lib.getDev curl)
        ];
      };
    });
  };
}
