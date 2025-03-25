{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-24.11";
    c3-lsp-src.url = "github:pherrymason/c3-lsp";
  };

  outputs = { self, nixpkgs, c3-lsp-src }: let
    system = "x86_64-linux";
    pkgs = import nixpkgs {
      inherit system;
      config.allowUnfree = true;
      config.cudaSupport = true;
    };

    # Define c3-lsp derivation
    c3-lsp = pkgs.stdenv.mkDerivation {
      pname = "c3-lsp";
      version = "latest";
      src = c3-lsp-src;

      nativeBuildInputs = with pkgs; [ cmake ninja clang ];

      buildPhase = ''
        cmake -B build -G Ninja
        cmake --build build
      '';

      installPhase = ''
        mkdir -p $out/bin
        cp build/c3-lsp $out/bin/
      '';
    };

  in {
    devShells."${system}".default = pkgs.mkShell {
      packages = with pkgs; [
        #### cuda dev packages ####
        cudaPackages.cudatoolkit
        cudaPackages.cudnn
        cudaPackages.cuda_cudart
        linuxPackages.nvidia_x11
        libGLU
        libGL

        #### odin packages ####
        # odin
        # ols
        #

        #### c3c packages ####
        c3c

        #### c3-lsp ####
        c3-lsp
      ];

      shellHook = ''
        export CUDA_PATH=${pkgs.cudaPackages.cudatoolkit}
        export LD_LIBRARY_PATH=${pkgs.linuxPackages.nvidia_x11}/lib:${pkgs.ncurses5}/lib
        export EXTRA_LDFLAGS="-L/lib -L${pkgs.linuxPackages.nvidia_x11}/lib"
        export EXTRA_CCFLAGS="-I/usr/include"
      '';
    };
  };
}
