// Generated from c:/Louistravail/4IF/GL/Comp/squelette-pld-comp/squelette-pld-comp/compiler/ifcc.g4 by ANTLR 4.13.1
import org.antlr.v4.runtime.tree.ParseTreeListener;

/**
 * This interface defines a complete listener for a parse tree produced by
 * {@link ifccParser}.
 */
public interface ifccListener extends ParseTreeListener {
	/**
	 * Enter a parse tree produced by {@link ifccParser#axiom}.
	 * @param ctx the parse tree
	 */
	void enterAxiom(ifccParser.AxiomContext ctx);
	/**
	 * Exit a parse tree produced by {@link ifccParser#axiom}.
	 * @param ctx the parse tree
	 */
	void exitAxiom(ifccParser.AxiomContext ctx);
	/**
	 * Enter a parse tree produced by {@link ifccParser#prog}.
	 * @param ctx the parse tree
	 */
	void enterProg(ifccParser.ProgContext ctx);
	/**
	 * Exit a parse tree produced by {@link ifccParser#prog}.
	 * @param ctx the parse tree
	 */
	void exitProg(ifccParser.ProgContext ctx);
	/**
	 * Enter a parse tree produced by {@link ifccParser#statement}.
	 * @param ctx the parse tree
	 */
	void enterStatement(ifccParser.StatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link ifccParser#statement}.
	 * @param ctx the parse tree
	 */
	void exitStatement(ifccParser.StatementContext ctx);
	/**
	 * Enter a parse tree produced by the {@code return_const_stmt}
	 * labeled alternative in {@link ifccParser#return_stmt}.
	 * @param ctx the parse tree
	 */
	void enterReturn_const_stmt(ifccParser.Return_const_stmtContext ctx);
	/**
	 * Exit a parse tree produced by the {@code return_const_stmt}
	 * labeled alternative in {@link ifccParser#return_stmt}.
	 * @param ctx the parse tree
	 */
	void exitReturn_const_stmt(ifccParser.Return_const_stmtContext ctx);
	/**
	 * Enter a parse tree produced by the {@code return_var_stmt}
	 * labeled alternative in {@link ifccParser#return_stmt}.
	 * @param ctx the parse tree
	 */
	void enterReturn_var_stmt(ifccParser.Return_var_stmtContext ctx);
	/**
	 * Exit a parse tree produced by the {@code return_var_stmt}
	 * labeled alternative in {@link ifccParser#return_stmt}.
	 * @param ctx the parse tree
	 */
	void exitReturn_var_stmt(ifccParser.Return_var_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link ifccParser#init_stmt}.
	 * @param ctx the parse tree
	 */
	void enterInit_stmt(ifccParser.Init_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link ifccParser#init_stmt}.
	 * @param ctx the parse tree
	 */
	void exitInit_stmt(ifccParser.Init_stmtContext ctx);
}