const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "xkb-layout",
        .root_module = b.createModule(.{
            .target = target,
            .optimize = optimize,
            .link_libc = true,
        }),
    });
    exe.root_module.addCSourceFiles(.{
        .files = &.{"main.c"},
        .flags = &.{
            "-std=gnu11",
            "-Wall",
            "-Wextra",
            "-pedantic",
        },
    });
    exe.root_module.linkSystemLibrary("X11", .{});

    b.installArtifact(exe);

    const run_exe = b.addRunArtifact(exe);
    run_exe.step.dependOn(b.getInstallStep());
    run_exe.addPassthruArgs();

    const run_step = b.step("run", "run xkb-layout");
    run_step.dependOn(&run_exe.step);
}
