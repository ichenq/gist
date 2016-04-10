-module(misc).
-export([qsort/1, pythag/1, perms/1]).

%%快排
qsort([]) 			-> [];
qsort([Pivot|T])	->
	qsort([X || X <- T, X < Pivot])
	++ [Pivot] ++
	qsort([X || X <- T, X >= Pivot]).

%%毕达哥拉斯元组
pythag(N) ->
	[{A,B,C} ||
	 A <- lists:seq(1, N),
	 B <- lists:seq(1, N),
	 C <- lists:seq(1, N),
	 A+B+C =< N,
	 A*A + B*B =:= C*C
	].

%%所有排列组合
perms([])	-> [[]];
perms(L)	-> [[H|T] || H <- L, T <- perms(L -- [H])].


filter1(true, H, P, T)	-> [H|filter(P, T)];
filter1(false, H, P, T) -> filter(P, T).

%%
filter(P, [H|T])	-> filter1(P(H), H, P, T);
filter(P, [])		-> [].

%%
filter2(P, [H|T]) ->
	case P(H) of
		true -> [H|filter2(P, T)];
		false -> filter2(P, T)
	end;
filter2(P, []) -> [].

%%奇数和偶数
odds_and_evens1(L) ->
	Odds = [X || X <- L, X rem 2 =:= 1],
	Evens = [X || X <- L, X rem 2 =:= 0],
	{Odds, Evens}.


odds_and_evens2(L) ->
	odds_and_evens_acc(L, [], []).

odds_and_evens_acc([H|T], Odds, Evens) ->
	case (H rem 2) of
		1 -> odds_and_evens_acc(T, [H|Odds], Evens);
		0 -> odds_and_evens_acc(T, Odds, [H|Evens])
	end;
odds_and_evens_acc([], Odds, Evens) ->
	{Odds, Evens}.

%%tuple to list	
my_tuple_to_list_impl(Max, Max, T) -> [element(Max, T)];
my_tuple_to_list_impl(I, Max, T) 	->
	[element(I,T) | my_tuple_to_list_impl(I+1, Max, T)].

my_tuple_to_list(T) ->
	my_tuple_to_list_impl(1, tuple_size(T), T).
