#pragma once

template <typename... Args>
struct TypeList {};

template <typename Signature>
struct ExtractReturnAndArgsImpl;

template <typename R, typename... Args>
struct ExtractReturnAndArgsImpl<R(Args...)> {
	using ReturnType = R;
	using ArgsList = TypeList<Args...>;
};

template <typename Signature>
using ExtractReturnAndArgsHelper = ExtractReturnAndArgsImpl<Signature>;
