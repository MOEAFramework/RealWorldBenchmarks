/* Copyright 2004, 2010-2012 Timothy W. Simpson and others.
import static java.lang.Math.pow;
/**
 * The General Aviation Aircraft (GAA) problem designed for use with the
 * MOEA Framework.
 */
public class GAA extends AbstractProblem {

	/**
	 * Constructs the General Aviation Aircraft (GAA) problem instance.
	 */
	public GAA() {
		super(27, 10, 1);
	}

	@Override
	public void evaluate(Solution solution) {
		double[] var = EncodingUtils.getReal(solution);
		double[] obj = new double[solution.getNumberOfObjectives()];
		double[] constr = new double[solution.getNumberOfConstraints()];

		double WEMP2_GOAL = 1900;

		solution.setObjectives(obj);
		solution.setConstraints(constr);
	}

	@Override
	public Solution newSolution() {
		Solution solution = new Solution(27, 10, 1);

		solution.setVariable(0, new RealVariable(0.24, 0.48));
		solution.setVariable(1, new RealVariable(7, 11));
		solution.setVariable(2, new RealVariable(0, 6));
		solution.setVariable(3, new RealVariable(5.5, 5.968));
		solution.setVariable(4, new RealVariable(19, 25));
		solution.setVariable(5, new RealVariable(85, 110));
		solution.setVariable(6, new RealVariable(14, 20));
		solution.setVariable(7, new RealVariable(3, 3.75));
		solution.setVariable(8, new RealVariable(0.46, 1));
		solution.setVariable(9, new RealVariable(0.24, 0.48));
		solution.setVariable(10, new RealVariable(7, 11));
		solution.setVariable(11, new RealVariable(0, 6));
		solution.setVariable(12, new RealVariable(5.5, 5.968));
		solution.setVariable(13, new RealVariable(19, 25));
		solution.setVariable(14, new RealVariable(85, 110));
		solution.setVariable(15, new RealVariable(14, 20));
		solution.setVariable(16, new RealVariable(3, 3.75));
		solution.setVariable(17, new RealVariable(0.46, 1));
		solution.setVariable(18, new RealVariable(0.24, 0.48));
		solution.setVariable(19, new RealVariable(7, 11));
		solution.setVariable(20, new RealVariable(0, 6));
		solution.setVariable(21, new RealVariable(5.5, 5.968));
		solution.setVariable(22, new RealVariable(19, 25));
		solution.setVariable(23, new RealVariable(85, 110));
		solution.setVariable(24, new RealVariable(14, 20));
		solution.setVariable(25, new RealVariable(3, 3.75));
		solution.setVariable(26, new RealVariable(0.46, 1));

		return solution;
	}

}