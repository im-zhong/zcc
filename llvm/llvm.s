; ModuleID = 'llvm.c'
source_filename = "llvm.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@i = dso_local constant i32 1, align 4
@j = dso_local global i32 8, align 4
@k = dso_local global i32 0, align 4

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @test_logical() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  store i32 1, ptr %1, align 4
  store i32 2, ptr %2, align 4
  %3 = load i32, ptr %1, align 4
  %4 = icmp ne i32 %3, 0
  br i1 %4, label %5, label %9

5:                                                ; preds = %0
  %6 = load i32, ptr %2, align 4
  %7 = icmp ne i32 %6, 0
  br i1 %7, label %8, label %9

8:                                                ; preds = %5
  store i32 3, ptr %2, align 4
  br label %9

9:                                                ; preds = %8, %5, %0
  %10 = load i32, ptr %2, align 4
  ret i32 %10
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @test_pointer() #0 {
  %1 = alloca i32, align 4
  %2 = alloca ptr, align 8
  %3 = alloca i32, align 4
  store i32 1, ptr %1, align 4
  store ptr %1, ptr %2, align 8
  %4 = load ptr, ptr %2, align 8
  %5 = load i32, ptr %4, align 4
  store i32 %5, ptr %3, align 4
  %6 = load i32, ptr %3, align 4
  ret i32 %6
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local double @test_double_pointer() #0 {
  %1 = alloca double, align 8
  %2 = alloca ptr, align 8
  %3 = alloca double, align 8
  store double 1.000000e+00, ptr %1, align 8
  store ptr %1, ptr %2, align 8
  %4 = load ptr, ptr %2, align 8
  %5 = load double, ptr %4, align 8
  store double %5, ptr %3, align 8
  %6 = load double, ptr %3, align 8
  ret double %6
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @test_struct_pointer() #0 {
  %1 = alloca ptr, align 8
  %2 = load ptr, ptr %1, align 8
  ret ptr %2
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @test_ssa_phi() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 1, ptr %1, align 4
  store i32 2, ptr %2, align 4
  %4 = load i32, ptr %2, align 4
  %5 = icmp ne i32 %4, 0
  br i1 %5, label %6, label %7

6:                                                ; preds = %0
  store i32 -1, ptr %1, align 4
  br label %8

7:                                                ; preds = %0
  store i32 3, ptr %1, align 4
  br label %8

8:                                                ; preds = %7, %6
  %9 = load i32, ptr %1, align 4
  store i32 %9, ptr %3, align 4
  %10 = load i32, ptr %3, align 4
  ret i32 %10
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @test_unsigned() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i8, align 1
  %3 = alloca i32, align 4
  %4 = alloca i8, align 1
  store i32 1, ptr %1, align 4
  store i8 1, ptr %2, align 1
  %5 = load i32, ptr %1, align 4
  %6 = load i8, ptr %2, align 1
  %7 = zext i8 %6 to i32
  %8 = add nsw i32 %5, %7
  store i32 %8, ptr %3, align 4
  store i8 1, ptr %4, align 1
  %9 = load i32, ptr %1, align 4
  %10 = load i8, ptr %4, align 1
  %11 = sext i8 %10 to i32
  %12 = add nsw i32 %9, %11
  store i32 %12, ptr %3, align 4
  %13 = load i32, ptr %3, align 4
  ret i32 %13
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @test_minus() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 1, ptr %1, align 4
  %4 = load i32, ptr %1, align 4
  %5 = sub nsw i32 0, %4
  store i32 %5, ptr %2, align 4
  %6 = load i32, ptr %2, align 4
  %7 = xor i32 %6, -1
  store i32 %7, ptr %3, align 4
  %8 = load i32, ptr %3, align 4
  ret i32 %8
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @test_mod() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 1, ptr %1, align 4
  store i32 2, ptr %2, align 4
  %4 = load i32, ptr %1, align 4
  %5 = load i32, ptr %2, align 4
  %6 = srem i32 %4, %5
  store i32 %6, ptr %3, align 4
  %7 = load i32, ptr %3, align 4
  ret i32 %7
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 2}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"clang version 15.0.7 (Fedora 15.0.7-1.fc37)"}
