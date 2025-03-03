
{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-24.11";
  };

  outputs = {
    self,
    nixpkgs,
  }: let
    system = "x86_64-linux";
    pkgs = import nixpkgs {
      inherit system;
      config.allowUnfree = true;
      config.cudaSupport = true;
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
